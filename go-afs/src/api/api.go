package api

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "libdfs/include/hdfs_filesystem.h"
hdfsFileInfo *getFileInfoByOffset(hdfsFileInfo *head, int offset)
{
    if (!head) return NULL;
    return head += offset;
}
#cgo LDFLAGS: -L${SRCDIR}/libdfs/lib -lhdfs -ljvm
*/
import "C"
import (
	"errors"
	"log"
	"os"
	//"path/filepath"
	//"runtime"
	"unsafe"
)

// 1. set some log flags
// 2. set comlog used in libdfs, note: since libdfs's C api
// did not wrap comlog api, you must implement this simple
// function-setComlog, then compile libdfs
func init() {
	log.SetPrefix("(api) [error] ")
	log.SetFlags(log.LstdFlags | log.Llongfile)
	//_, file, _, _ := runtime.Caller(0)
	//dir := C.CString(filepath.Dir(file) + "/conf")
	dir := C.CString("./conf")
	defer freeCString(dir)
	filename := C.CString("log.conf")
	defer freeCString(filename)
	C.setComlog(dir, filename)
}

type HdfsBuilderConfig struct {
	Endpoint      string
	Port          int
	Ugi           string
	ConnTimeout   string
	ReadTimeout   string
	WriteTimeout  string
	MaxRetryCount string
	AuthMethod    string
}

// wrap the hdfs api
type FileSystem struct {
	BuilderConfig HdfsBuilderConfig
	Fs            C.hdfsFS
}

// represent an opened file
type File struct {
	Fp          C.hdfsFile
	Path        string
	Flags       int
	BufferSize  int
	Replication int8
	BlockSize   int
	Permission  int
	IsAppend    bool
	IsOverWrite bool
}

// for a dir or file or symlink
type PathInfo struct {
	Kind        string
	Name        string
	Owner       string
	Group       string
	Replication int8
	Size        int64
	BlockSize   int64
	Permissions int8
	LastMod     int64
	LastAccess  int64
}

// free memory when copy go string to c char*
func freeCString(p *C.char) {
	C.free(unsafe.Pointer(p))
}

// note: can not free memory of the hdfs builder here
func NewFileSystem(config HdfsBuilderConfig) (*FileSystem, error) {
	fs := &FileSystem{}
	fs.BuilderConfig = config

	var bld *C.struct_hdfsBuilder
	bld = C.hdfsNewBuilder()

	// set url
	url := C.CString(config.Endpoint)
	defer freeCString(url)
	C.hdfsBuilderSetNameNode(bld, url)

	// set port
	C.hdfsBuilderSetNameNodePort(bld, C.tPort(config.Port))

	// set ugi
	ugi := C.CString(config.Ugi)
	defer freeCString(ugi)
	C.hdfsBuilderSetUserName(bld, ugi)

	// set conn timeout
	connTimeoutKey := C.CString("dfs.client.connect.timeout")
	defer freeCString(connTimeoutKey)
	connTimeoutValue := C.CString(config.ConnTimeout)
	defer freeCString(connTimeoutValue)
	C.hdfsBuilderConfSetStr(bld, connTimeoutKey, connTimeoutValue)

	// set read timeout
	readTimeoutKey := C.CString("dfs.client.read.timeout")
	defer freeCString(readTimeoutKey)
	readTimeoutValue := C.CString(config.ReadTimeout)
	defer freeCString(readTimeoutValue)
	C.hdfsBuilderConfSetStr(bld, readTimeoutKey, readTimeoutValue)

	// set write timeout
	writeTimeoutKey := C.CString("dfs.client.write.timeout")
	defer freeCString(writeTimeoutKey)
	writeTimeoutValue := C.CString(config.WriteTimeout)
	defer freeCString(writeTimeoutValue)
	C.hdfsBuilderConfSetStr(bld, writeTimeoutKey, writeTimeoutValue)

	// set max retry count
	maxRetryKey := C.CString("dfs.client.max.retries")
	defer freeCString(maxRetryKey)
	maxRetryValue := C.CString(config.MaxRetryCount)
	defer freeCString(maxRetryValue)
	C.hdfsBuilderConfSetStr(bld, maxRetryKey, maxRetryValue)

	// set auth mode
	authMethodKey := C.CString("dfs.client.auth.method")
	defer freeCString(authMethodKey)
	authMethodValue := C.CString(config.AuthMethod)
	defer freeCString(authMethodValue)
	C.hdfsBuilderConfSetStr(bld, authMethodKey, authMethodValue)

	ret, err := C.hdfsBuilderConnect(bld)
	if err != nil {
		log.Printf("failed to connect to file system %s: %v\n", config.Endpoint, err)
		return nil, errors.New("create file system err")
	}
	fs.Fs = ret
	return fs, nil
}

// open file
func (fs *FileSystem) OpenFile(path string, flags int, bufferSize int, replication int8, blockSize int) (*File, error) {
	return fs.OpenFileExtra(path, flags, bufferSize, replication, blockSize, 755, false, true)
}

// open file with extra flags
func (fs *FileSystem) OpenFileExtra(path string, flags int, bufferSize int, replication int8, blockSize int, perm int, isAppend bool, isOverWrite bool) (*File, error) {
	file := &File{
		Path:        path,
		Flags:       flags,
		BufferSize:  bufferSize,
		Replication: replication,
		BlockSize:   blockSize,
		Permission:  perm,
		IsAppend:    isAppend,
		IsOverWrite: isOverWrite,
	}
	pathString := C.CString(path)
	defer freeCString(pathString)
	flagAppend := 0
	flagOverWrite := 0
	if isAppend {
		flagAppend = 1
	}
	if isOverWrite {
		flagOverWrite = 1
	}
	ret, err := C.hdfsOpenFileExtra(fs.Fs, pathString, C.int(flags), C.int(bufferSize), C.short(replication), C.tSize(blockSize), C.int32_t(perm), C.bool(flagAppend), C.bool(flagOverWrite))
	// notice: we can not use err to check if err return
	// since the C api will return opened file fp with err
	// not nil ...
	if ret == nil {
		log.Printf("failed to open file %s: %v\n", path, err)
		return nil, errors.New("open file error")
	}
	file.Fp = ret
	return file, nil
}

// read file
func (fs *FileSystem) ReadFile(file *File, length int) ([]byte, error) {
	return fs.ReadFileFromOffset(file, 0, length)
}

// read file from offset
func (fs *FileSystem) ReadFileFromOffset(file *File, position int64, length int) ([]byte, error) {
	// note: read size should not be too large or hdfsPRead will return err
	// usually, if you want to read large file, you should split it into multiple
	// parts to read
	buffer := C.malloc(C.size_t(length))
	defer C.free(buffer)
	ret, err := C.hdfsPread(fs.Fs, file.Fp, C.tOffset(position), buffer, C.tSize(length))
	if err != nil || int(ret) < 0 {
		log.Printf("read file %s error: %v\n", file.Path, err)
		return nil, errors.New("read file failed")
	}
	return C.GoBytes(buffer, C.int(ret)), nil
}

// write to file
func (fs *FileSystem) WriteFile(file *File, buffer []byte, length int) (int, error) {
	buf := C.CBytes(buffer)
	defer C.free(buf)
	ret, err := C.hdfsWrite(fs.Fs, file.Fp, buf, C.tSize(length))
	if err != nil || int(ret) < 0 {
		log.Printf("write file %s err: %v\n", file.Path, err)
		return int(ret), errors.New("write file error")
	}
	return int(ret), nil
}

// close file
func (fs *FileSystem) CloseFile(file *File) error {
	ret, err := C.hdfsCloseFile(fs.Fs, file.Fp)
	if err != nil || int(ret) < 0 {
		log.Printf("close file %s failed: %v\n", file.Path, err)
		return errors.New("close file err")
	}
	return nil
}

// create file
func (fs *FileSystem) CreateFile(path string, perm int, replication int, block_size int) error {
	pathString := C.CString(path)
	defer freeCString(pathString)
	ret, err := C.hdfsCreate(fs.Fs, pathString, C.int32_t(perm), C.int32_t(replication), C.int32_t(block_size))
	if err != nil || int(ret) < 0 {
		log.Printf("failed to create file %s: %v\n", path, err)
		return errors.New("create file err")
	}
	return nil
}

// create dir
func (fs *FileSystem) CreateDir(path string) error {
	pathString := C.CString(path)
	defer freeCString(pathString)
	ret, err := C.hdfsCreateDirectory(fs.Fs, pathString)
	if err != nil || int(ret) < 0 {
		log.Printf("failed to create dir %s: %v\n", path, err)
		return errors.New("create dir err")
	}
	return nil
}

// delete path
func (fs *FileSystem) DeletePath(path string) error {
	pathString := C.CString(path)
	defer freeCString(pathString)
	ret, err := C.hdfsDelete(fs.Fs, pathString)
	if err != nil || int(ret) < 0 {
		log.Printf("failed to delete path %s: %v\n", path, err)
		return errors.New("delete path err")
	}
	return nil
}

// flush
func (fs *FileSystem) FlushFile(file *File) error {
	ret, err := C.hdfsFlush(fs.Fs, file.Fp)
	if err != nil || int(ret) < 0 {
		log.Printf("failed to flush file %s: %v\n", file.Path, err)
		return errors.New("flush file err")
	}
	return nil
}

// sync
func (fs *FileSystem) SyncFile(file *File) error {
	ret, err := C.hdfsHSync(fs.Fs, file.Fp)
	if err != nil || int(ret) < 0 {
		log.Printf("failed to sync file %s: %v\n", file.Path, err)
		return errors.New("sync file err")
	}
	return nil
}

// checksum
func (fs *FileSystem) CheckSumMD5(path string) (string, error) {
	pathString := C.CString(path)
	defer freeCString(pathString)
	s, err := C.hdfsGetFileCheckSumMD5(fs.Fs, pathString)
	if err != nil {
		log.Printf("failed get checksum for %s: %v\n", path, err)
		return "", errors.New("checksum md5 err")
	}
	return C.GoString(s), nil
}

// rename
func (fs *FileSystem) Rename(src, dst string) error {
	srcString := C.CString(src)
	defer freeCString(srcString)
	dstString := C.CString(dst)
	defer freeCString(dstString)
	ret, err := C.hdfsRename(fs.Fs, srcString, dstString)
	if err != nil || int(ret) < 0 {
		log.Printf("failed to rename %s to %s: %v\n", src, dst, err)
		return errors.New("rename err")
	}
	return nil
}

// move between the same file system
func (fs *FileSystem) MovePath(src, dst string) error {
	srcString := C.CString(src)
	defer freeCString(srcString)
	dstString := C.CString(dst)
	defer freeCString(dstString)
	ret, err := C.hdfsMove(fs.Fs, srcString, fs.Fs, dstString)
	if err != nil || int(ret) < 0 {
		log.Printf("failed to move %s to %s: %v\n", src, dst, err)
		return errors.New("move path err")
	}
	return nil
}

// copy between the same file system
func (fs *FileSystem) CopyPath(src, dst string) error {
	srcString := C.CString(src)
	defer freeCString(srcString)
	dstString := C.CString(dst)
	defer freeCString(dstString)
	ret, err := C.hdfsCopy(fs.Fs, srcString, fs.Fs, dstString)
	if err != nil || int(ret) < 0 {
		log.Printf("failed to copy %s to %d: %v\n", src, dst, err)
		return errors.New("copy path err")
	}
	return nil
}

// isExist
func (fs *FileSystem) IsExists(path string) (bool, error) {
	pathString := C.CString(path)
	defer freeCString(pathString)
	ret, err := C.hdfsExists(fs.Fs, pathString)
	if err != nil || int(ret) < 0 {
		log.Printf("failed to check exist for %s: %v\n", path, err)
		return false, errors.New("isexist err")
	}
	if ret == 0 {
		return true, nil
	}
	return false, nil
}

// convert cinfo to PathInfo
func convertToPathInfo(cinfo *C.hdfsFileInfo) *PathInfo {
	info := &PathInfo{}
	info.Kind = string(rune(cinfo.mKind))
	info.Name = C.GoString(cinfo.mName)
	info.LastMod = int64(cinfo.mLastMod)
	info.Size = int64(cinfo.mSize)
	info.Replication = int8(cinfo.mReplication)
	info.BlockSize = int64(cinfo.mBlockSize)
	info.Owner = C.GoString(cinfo.mOwner)
	info.Group = C.GoString(cinfo.mGroup)
	info.Permissions = int8(cinfo.mPermissions)
	info.LastAccess = int64(cinfo.mLastAccess)
	return info
}

// list dir
func (fs *FileSystem) ListDir(path string) ([]*PathInfo, error) {
	pathString := C.CString(path)
	defer freeCString(pathString)
	var num int
	cinfos, err := C.hdfsListDirectory(fs.Fs, pathString, (*C.int)(unsafe.Pointer(&num)))
	if err != nil {
		log.Printf("failed to list dir %s: %v\n", path, err)
		return nil, errors.New("list dir err")
	}
	var infos []*PathInfo
	for i := 0; i < num; i++ {
		infos = append(infos, convertToPathInfo(C.getFileInfoByOffset(cinfos, C.int(i))))
	}
	C.hdfsFreeFileInfo(cinfos, C.int(num))
	return infos, nil
}

// get path info
func (fs *FileSystem) GetPathInfo(path string) (*PathInfo, error) {
	info := &PathInfo{}
	pathString := C.CString(path)
	defer freeCString(pathString)
	cinfo, err := C.hdfsGetPathInfo(fs.Fs, pathString)
	if err != nil {
		log.Printf("failed to get path info for %s: %v\n", path, err)
		return nil, errors.New("get path info err")
	}
	info = convertToPathInfo(cinfo)
	C.hdfsFreeFileInfo(cinfo, 1)
	return info, nil
}

// seek
func (fs *FileSystem) SeekFile(file *File, position int) error {
	ret, err := C.hdfsSeek(fs.Fs, file.Fp, C.tOffset(position))
	if err != nil || int(ret) < 0 {
		log.Printf("failed to seek file %s: %v", file.Path, err)
		return errors.New("seek file err")
	}
	return nil
}

// wrap WriteFile for easy use
func (fs *FileSystem) Write(path string, buffer []byte, length int, isAppend bool) (int, error) {
	flags := os.O_WRONLY
	if isAppend {
		flags = flags | os.O_APPEND
	}
	file, err := fs.OpenFile(path, flags, 0, 0, 0)
	if file == nil {
		log.Printf("Write: failed to open %s\n", path)
		return -1, err
	}
	var l int
	l, err = fs.WriteFile(file, buffer, length)
	err = fs.CloseFile(file)
	return l, err

}

// wrap ReadFile for easy use
func (fs *FileSystem) Read(path string, position int64, length int) ([]byte, error) {
	file, err := fs.OpenFile(path, os.O_RDONLY, 0, 0, 0)
	if file == nil {
		log.Printf("Read: failed to open %s\n", path)
		return nil, err
	}
	var buffer []byte
	buffer, err = fs.ReadFileFromOffset(file, position, length)
	err = fs.CloseFile(file)
	return buffer, err
}

func (fs *FileSystem) Disconnect() error {
	ret, err := C.hdfsDisconnect(fs.Fs)
	if err != nil || int(ret) < 0 {
		log.Printf("disconnect err")
		return err
	}
	return nil
}
