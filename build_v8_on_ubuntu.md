## Environment: Ubuntu
Working dir (~)
> Credit to @j4k0xb

## Environment preparation:
```
sudo apt-get update
sudo apt-get install ninja-build clang pkg-config
```
## Checkout depot_tools:
```
cd ~
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
export PATH=~/depot_tools:$PATH
```

## Fetch v8
```
cd ~
fetch v8
gclient sync
git checkout refs/tags/10.6.194.26
gclient sync -D 
```
# Apply patch (make sure it is success)
```
wget https://raw.githubusercontent.com/j4k0xb/View8/main/Disassembler/v8.patch 
mv v8.patch ~/v8/
cd ~/v8/
git status
git apply v8.patch
git status
```

## Create build
```
cd ~/v8
./tools/dev/v8gen.py x64.release
```

## Modify build arguments
```
nano out.gn/x64.release/args.gn
```

> Change it to below lines (default):
```
    dcheck_always_on = false
    is_component_build = false
    is_debug = false
    target_cpu = "x64"
    use_custom_libcxx = false
    v8_monolithic = true
    v8_use_external_startup_data = false
    v8_static_library = true
    v8_enable_disassembler = true
    v8_enable_object_print = true
```
> If it is **Node** app (not the **electron**), then add following line:
  ```
    v8_enable_pointer_compression = false
  ```

## Build v8 static library:
```
cd ~/v8
ninja -C out.gn/x64.release v8_monolith
```

## Build disassember
```
cd ~/v8
wget https://raw.githubusercontent.com/j4k0xb/View8/refs/heads/main/Disassembler/v8dasm.cpp
```
> For electron:
```
clang++ v8dasm.cpp -g -std=c++17 -Iinclude -Lout.gn/x64.release/obj -lv8_libbase -lv8_libplatform -lv8_monolith -o v8dasm -DV8_COMPRESS_POINTERS -ldl -pthread
```
> For Node:
```
clang++ v8dasm.cpp -g -std=c++17 -Iinclude -Lout.gn/x64.release/obj -lv8_libbase -lv8_libplatform -lv8_monolith -o v8dasm
```
## Disassembler a file:
```
cd ~
git clone https://github.com/j4k0xb/View8.git
cd View8
cp ~/v8/v8dasm Disassembler
chmod +x Disassembler/v8dasm
pip install parse
python view8.py test.jsc test.dasm --path Disassembler/v8dasm
```
