package api

import (
	"os"
	"testing"
)

var config = HdfsBuilderConfig{
	Endpoint: "honour.afstest.afs.baidu.com",
	Port:     61000,
	Ugi:      "username,password",
}
var fs, _ = NewFileSystem(config)

func createFileSystem() (*FileSystem, error) {
	config := HdfsBuilderConfig{
		Endpoint: "honour.afstest.afs.baidu.com",
		Port:     61000,
		Ugi:      "username,password",
	}
	return NewFileSystem(config)
}

/*
func TestCreateFileSystem(t *testing.T) {
	fs, err := createFileSystem()
	if err != nil || fs == nil {
		t.Error("create file system failed")
	}
	fs.Disconnect()
}
*/

func TestFileOperations(t *testing.T) {
	var err error
	var info *PathInfo
	//var fs *FileSystem
	var file *File
	var buffer []byte
	// CreateFile, GetPathInfo, DeletePath
	err = fs.CreateFile("~/test.test", 755, -1, -1)
	if err != nil {
		t.Error("create file failed")
	}
	info, err = fs.GetPathInfo("~/test.test")
	if err != nil || info.Kind != "F" {
		t.Error("failed path info")
	}
	err = fs.DeletePath("~/test.test")
	if err != nil {
		t.Error("delete file failed")
	}

	// CreateDir, ListDir, DeletePath
	err = fs.CreateDir("~/test.dir")
	if err != nil {
		t.Error("create dir failed")
	}
	err = fs.CreateFile("~/test.dir/test.file", 755, -1, -1)
	if err != nil {
		t.Error("create file failed")
	}
	info, err = fs.GetPathInfo("~/test.dir")
	if err != nil || info.Kind != "D" {
		t.Error("not dir")
	}
	infos, err := fs.ListDir("~/test.dir")
	if err != nil || len(infos) != 1 {
		t.Error("dir info len")
	}
	err = fs.DeletePath("~/test.dir")
	if err != nil {
		t.Error("delete dir failed")
	}

	// OpenFile, WriteFile, ReadFile, CloseFile
	err = fs.CreateFile("~/test.test", 755, -1, -1)
	if err != nil {
		t.Error("create file failed")
	}
	// write
	file, err = fs.OpenFile("~/test.test", os.O_WRONLY|os.O_APPEND, 0, 0, 0)
	if err != nil {
		t.Error("failed to open file")
	}
	buffer = []byte("a line\n")
	l := len(buffer)
	_, err = fs.WriteFile(file, buffer, l)
	if err != nil {
		t.Error("failed to write file")
	}
	err = fs.CloseFile(file)
	if err != nil {
		t.Error("failed to close file")
	}
	// read
	file, err = fs.OpenFile("~/test.test", os.O_RDONLY, 0, 0, 0)
	if err != nil {
		t.Error("failed to open to read")
	}
	buffer, err = fs.ReadFile(file, l)
	if err != nil {
		t.Error("failed to read")
	}
	if string(buffer) != "a line\n" {
		t.Error("read content not the same")
	}
	err = fs.CloseFile(file)
	if err != nil {
		t.Error("failed to close file")
	}
	file, err = fs.OpenFile("~/test.test", os.O_RDONLY, 0, 0, 0)
	if err != nil {
		t.Error("failed to open to read")
	}
	buffer, err = fs.ReadFileFromOffset(file, 2, 1)
	if err != nil {
		t.Error("failed to read")
	}
	if string(buffer) != "l" {
		t.Error("read from offset content not the same")
	}
	err = fs.CloseFile(file)
	if err != nil {
		t.Error("failed to close file")
	}
	err = fs.DeletePath("~/test.test")
	if err != nil {
		t.Error("failed to delete ~/test.test")
	}
	// isExist
	err = fs.CreateFile("~/test.test", 755, -1, -1)
	if err != nil {
		t.Error("create file failed")
	}
	var temp bool
	temp, err = fs.IsExists("~/test.test")
	if err != nil || temp == false {
		t.Error("isexists failed")
	}
	fs.DeletePath("~/test.test")
}

func TestReadWriteDirect(t *testing.T) {
	path := "~/testfile.temp"
	var err error
	buffer := []byte("a line\n")
	_, err = fs.Write(path, buffer, len(buffer), false)
	if err != nil {
		t.Error("failed write")
	}
	buffer = []byte("fuck\n")
	_, err = fs.Write(path, buffer, len(buffer), false)
	if err != nil {
		t.Error("failed write")
	}
	var info *PathInfo
	info, err = fs.GetPathInfo(path)
	if err != nil || info.Size != int64(len(buffer)) {
		t.Error("failed get path info")
	}
	_, err = fs.Write(path, buffer, len(buffer), true)
	if err != nil {
		t.Error("failed write")
	}
	info, err = fs.GetPathInfo(path)
	if err != nil || info.Size != int64(len(buffer)*2) {
		t.Error("failed get path info")
	}

	buffer, err = fs.Read(path, 0, 1)
	if err != nil || string(buffer) != "f" {
		t.Error("failed to read")
	}
	buffer, err = fs.Read(path, 2, 2)
	if err != nil || string(buffer) != "ck" {
		t.Error("failed to read")
	}
	fs.DeletePath(path)
}
