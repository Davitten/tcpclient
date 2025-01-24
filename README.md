# tcpclient

# Generating protos

## C++

```
protoc --cpp_out=. protos/addressbook.proto
```


## Python

```
protoc --python_out=../../PycharmProjects/PythonTCPClient/ protos/addressbook.proto
```

and 

```
protoc --pyi_out=../../PycharmProjects/PythonTCPClient/ protos/addressbook.proto
```

For devs install spdlog:
```sudo apt install libspdlog-dev```