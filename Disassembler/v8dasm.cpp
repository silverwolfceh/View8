#include <fstream>
#include <iostream>
#include <string>

#include "libplatform/libplatform.h"
#include "v8.h"

#pragma comment(lib, "v8_libbase.lib")
#pragma comment(lib, "v8_libplatform.lib")
#pragma comment(lib, "wee8.lib")

#pragma comment(lib, "secur32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dmoguids.lib")
#pragma comment(lib, "wmcodecdspuuid.lib")
#pragma comment(lib, "msdmo.lib")
#pragma comment(lib, "Strmiids.lib")
#pragma comment(lib, "DbgHelp.lib")

using namespace v8;

static Isolate* isolate = nullptr;

static void loadBytecode(uint8_t* bytecodeBuffer, int length) {
  // Load code into code cache.
  ScriptCompiler::CachedData* cached_data =
      new ScriptCompiler::CachedData(bytecodeBuffer, length);

  // Create dummy source.
  ScriptOrigin origin(isolate, String::NewFromUtf8Literal(isolate, "code.jsc"));
  ScriptCompiler::Source source(String::NewFromUtf8Literal(isolate, "\"ಠ_ಠ\""),
                                origin, cached_data);

  // Compile code from code cache to print disassembly.
  MaybeLocal<UnboundScript> script = ScriptCompiler::CompileUnboundScript(
      isolate, &source, ScriptCompiler::kConsumeCodeCache);
}

static void readAllBytes(const std::string& file, std::vector<char>& buffer) {
  std::ifstream infile(file, std::ios::binary);

  infile.seekg(0, infile.end);
  size_t length = infile.tellg();
  infile.seekg(0, infile.beg);

  if (length > 0) {
    buffer.resize(length);
    infile.read(&buffer[0], length);
  }
}

int main(int argc, char* argv[]) {
  V8::SetFlagsFromString("--no-lazy --no-flush-bytecode");

  V8::InitializeICU();
  std::unique_ptr<Platform> platform = platform::NewDefaultPlatform();
  V8::InitializePlatform(platform.get());
  V8::Initialize();

  Isolate::CreateParams create_params;
  create_params.array_buffer_allocator =
      ArrayBuffer::Allocator::NewDefaultAllocator();

  isolate = Isolate::New(create_params);
  Isolate::Scope isolate_scope(isolate);
  HandleScope scope(isolate);

  std::vector<char> data;
  readAllBytes(argv[1], data);
  loadBytecode((uint8_t*)data.data(), data.size());
}
