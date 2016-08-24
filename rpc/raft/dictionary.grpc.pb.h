// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: dictionary.proto
#ifndef GRPC_dictionary_2eproto__INCLUDED
#define GRPC_dictionary_2eproto__INCLUDED

#include "dictionary.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/proto_utils.h>
#include <grpc++/impl/codegen/rpc_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/status.h>
#include <grpc++/impl/codegen/stub_options.h>
#include <grpc++/impl/codegen/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class RpcService;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace Tan {

// Interface exported by the server.
class Dictionary GRPC_FINAL {
 public:
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status GetOne(::grpc::ClientContext* context, const ::Tan::Word& request, ::Tan::Meaning* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::Tan::Meaning>> AsyncGetOne(::grpc::ClientContext* context, const ::Tan::Word& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::Tan::Meaning>>(AsyncGetOneRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientReaderInterface< ::Tan::Meaning>> SendStreamList(::grpc::ClientContext* context, const ::Tan::Word& request) {
      return std::unique_ptr< ::grpc::ClientReaderInterface< ::Tan::Meaning>>(SendStreamListRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::Tan::Meaning>> AsyncSendStreamList(::grpc::ClientContext* context, const ::Tan::Word& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::Tan::Meaning>>(AsyncSendStreamListRaw(context, request, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientWriterInterface< ::Tan::Word>> GetStreamList(::grpc::ClientContext* context, ::Tan::Meaning* response) {
      return std::unique_ptr< ::grpc::ClientWriterInterface< ::Tan::Word>>(GetStreamListRaw(context, response));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::Tan::Word>> AsyncGetStreamList(::grpc::ClientContext* context, ::Tan::Meaning* response, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::Tan::Word>>(AsyncGetStreamListRaw(context, response, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::Tan::Word, ::Tan::Meaning>> StreamList(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::Tan::Word, ::Tan::Meaning>>(StreamListRaw(context));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::Tan::Word, ::Tan::Meaning>> AsyncStreamList(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::Tan::Word, ::Tan::Meaning>>(AsyncStreamListRaw(context, cq, tag));
    }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::Tan::Meaning>* AsyncGetOneRaw(::grpc::ClientContext* context, const ::Tan::Word& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientReaderInterface< ::Tan::Meaning>* SendStreamListRaw(::grpc::ClientContext* context, const ::Tan::Word& request) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::Tan::Meaning>* AsyncSendStreamListRaw(::grpc::ClientContext* context, const ::Tan::Word& request, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientWriterInterface< ::Tan::Word>* GetStreamListRaw(::grpc::ClientContext* context, ::Tan::Meaning* response) = 0;
    virtual ::grpc::ClientAsyncWriterInterface< ::Tan::Word>* AsyncGetStreamListRaw(::grpc::ClientContext* context, ::Tan::Meaning* response, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientReaderWriterInterface< ::Tan::Word, ::Tan::Meaning>* StreamListRaw(::grpc::ClientContext* context) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::Tan::Word, ::Tan::Meaning>* AsyncStreamListRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) = 0;
  };
  class Stub GRPC_FINAL : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status GetOne(::grpc::ClientContext* context, const ::Tan::Word& request, ::Tan::Meaning* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::Tan::Meaning>> AsyncGetOne(::grpc::ClientContext* context, const ::Tan::Word& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::Tan::Meaning>>(AsyncGetOneRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientReader< ::Tan::Meaning>> SendStreamList(::grpc::ClientContext* context, const ::Tan::Word& request) {
      return std::unique_ptr< ::grpc::ClientReader< ::Tan::Meaning>>(SendStreamListRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::Tan::Meaning>> AsyncSendStreamList(::grpc::ClientContext* context, const ::Tan::Word& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::Tan::Meaning>>(AsyncSendStreamListRaw(context, request, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientWriter< ::Tan::Word>> GetStreamList(::grpc::ClientContext* context, ::Tan::Meaning* response) {
      return std::unique_ptr< ::grpc::ClientWriter< ::Tan::Word>>(GetStreamListRaw(context, response));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriter< ::Tan::Word>> AsyncGetStreamList(::grpc::ClientContext* context, ::Tan::Meaning* response, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncWriter< ::Tan::Word>>(AsyncGetStreamListRaw(context, response, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientReaderWriter< ::Tan::Word, ::Tan::Meaning>> StreamList(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriter< ::Tan::Word, ::Tan::Meaning>>(StreamListRaw(context));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::Tan::Word, ::Tan::Meaning>> AsyncStreamList(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::Tan::Word, ::Tan::Meaning>>(AsyncStreamListRaw(context, cq, tag));
    }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    ::grpc::ClientAsyncResponseReader< ::Tan::Meaning>* AsyncGetOneRaw(::grpc::ClientContext* context, const ::Tan::Word& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientReader< ::Tan::Meaning>* SendStreamListRaw(::grpc::ClientContext* context, const ::Tan::Word& request) GRPC_OVERRIDE;
    ::grpc::ClientAsyncReader< ::Tan::Meaning>* AsyncSendStreamListRaw(::grpc::ClientContext* context, const ::Tan::Word& request, ::grpc::CompletionQueue* cq, void* tag) GRPC_OVERRIDE;
    ::grpc::ClientWriter< ::Tan::Word>* GetStreamListRaw(::grpc::ClientContext* context, ::Tan::Meaning* response) GRPC_OVERRIDE;
    ::grpc::ClientAsyncWriter< ::Tan::Word>* AsyncGetStreamListRaw(::grpc::ClientContext* context, ::Tan::Meaning* response, ::grpc::CompletionQueue* cq, void* tag) GRPC_OVERRIDE;
    ::grpc::ClientReaderWriter< ::Tan::Word, ::Tan::Meaning>* StreamListRaw(::grpc::ClientContext* context) GRPC_OVERRIDE;
    ::grpc::ClientAsyncReaderWriter< ::Tan::Word, ::Tan::Meaning>* AsyncStreamListRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) GRPC_OVERRIDE;
    const ::grpc::RpcMethod rpcmethod_GetOne_;
    const ::grpc::RpcMethod rpcmethod_SendStreamList_;
    const ::grpc::RpcMethod rpcmethod_GetStreamList_;
    const ::grpc::RpcMethod rpcmethod_StreamList_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status GetOne(::grpc::ServerContext* context, const ::Tan::Word* request, ::Tan::Meaning* response);
    virtual ::grpc::Status SendStreamList(::grpc::ServerContext* context, const ::Tan::Word* request, ::grpc::ServerWriter< ::Tan::Meaning>* writer);
    virtual ::grpc::Status GetStreamList(::grpc::ServerContext* context, ::grpc::ServerReader< ::Tan::Word>* reader, ::Tan::Meaning* response);
    virtual ::grpc::Status StreamList(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::Tan::Meaning, ::Tan::Word>* stream);
  };
  template <class BaseClass>
  class WithAsyncMethod_GetOne : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_GetOne() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_GetOne() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetOne(::grpc::ServerContext* context, const ::Tan::Word* request, ::Tan::Meaning* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGetOne(::grpc::ServerContext* context, ::Tan::Word* request, ::grpc::ServerAsyncResponseWriter< ::Tan::Meaning>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_SendStreamList : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_SendStreamList() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_SendStreamList() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SendStreamList(::grpc::ServerContext* context, const ::Tan::Word* request, ::grpc::ServerWriter< ::Tan::Meaning>* writer) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestSendStreamList(::grpc::ServerContext* context, ::Tan::Word* request, ::grpc::ServerAsyncWriter< ::Tan::Meaning>* writer, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncServerStreaming(1, context, request, writer, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_GetStreamList : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_GetStreamList() {
      ::grpc::Service::MarkMethodAsync(2);
    }
    ~WithAsyncMethod_GetStreamList() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetStreamList(::grpc::ServerContext* context, ::grpc::ServerReader< ::Tan::Word>* reader, ::Tan::Meaning* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGetStreamList(::grpc::ServerContext* context, ::grpc::ServerAsyncReader< ::Tan::Meaning, ::Tan::Word>* reader, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncClientStreaming(2, context, reader, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_StreamList : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_StreamList() {
      ::grpc::Service::MarkMethodAsync(3);
    }
    ~WithAsyncMethod_StreamList() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status StreamList(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::Tan::Meaning, ::Tan::Word>* stream) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestStreamList(::grpc::ServerContext* context, ::grpc::ServerAsyncReaderWriter< ::Tan::Meaning, ::Tan::Word>* stream, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncBidiStreaming(3, context, stream, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_GetOne<WithAsyncMethod_SendStreamList<WithAsyncMethod_GetStreamList<WithAsyncMethod_StreamList<Service > > > > AsyncService;
  template <class BaseClass>
  class WithGenericMethod_GetOne : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_GetOne() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_GetOne() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetOne(::grpc::ServerContext* context, const ::Tan::Word* request, ::Tan::Meaning* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_SendStreamList : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_SendStreamList() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_SendStreamList() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SendStreamList(::grpc::ServerContext* context, const ::Tan::Word* request, ::grpc::ServerWriter< ::Tan::Meaning>* writer) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_GetStreamList : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_GetStreamList() {
      ::grpc::Service::MarkMethodGeneric(2);
    }
    ~WithGenericMethod_GetStreamList() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetStreamList(::grpc::ServerContext* context, ::grpc::ServerReader< ::Tan::Word>* reader, ::Tan::Meaning* response) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_StreamList : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_StreamList() {
      ::grpc::Service::MarkMethodGeneric(3);
    }
    ~WithGenericMethod_StreamList() GRPC_OVERRIDE {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status StreamList(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::Tan::Meaning, ::Tan::Word>* stream) GRPC_FINAL GRPC_OVERRIDE {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
};

}  // namespace Tan


#endif  // GRPC_dictionary_2eproto__INCLUDED
