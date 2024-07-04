# Http++

Http++ is an http(s) server implementation for C++. It can be used to quickly start creating a backend with C++, without the hastle of implementing boilerplate code.

## Features

### Header parsing
### Body parsing
### Handles GET and POST natively
### API Route parsing
### API Versioning
### Error handling

## Installation

In order to start using Http++ you will need to do the following:

- Place the contents of the `headers` directory into your `headers` folder
- Place the contents of the `sources` directory into your `sources` folder
- Make sure to tweak the connection.cpp as per your needs.
- When compiling, make sure to pass the `connection.cpp` file to the compiler.
- That is it. You can now continue onto making your app.

## Creating an app

Http++ allows for basic API creation. It does not support **REST**. Available API operations are:
- GET
- POST

Taking a look at `source.cpp` you can get the general idea of how Http++ works, but we will provide examples here too.

### Creating a server listener:

```cpp
Server::Listen(1313,HandlePost,HandleGet,1000);
```
- The first argument is the PORT.
- The second `HandlePost` is a function pointer. This function will be executed every time a POST is sent to the server.
- The third `HandleGet` is a function pointer. This function will be executed every time a GET is sent to the server.
- The final argument is the backlog.

The `Server::Listen` function is a blocking function. Every action from now on should be handled inside of `HandlePost` and `HandleGet` functions.

### Creating the HandlePost and HandleGet functions

```cpp
short HandlePost(int clientSocket, json requestJSON,std::string requestRoute,json headers){
    json responseJSON = {};
    Response::RespondJSON(0,0,responseJSON);
    return 0;
}
```
The HandlePost function must take in:
- clientSocket
- requestJSON
- requestRoute
- JSON headers

```cpp
short HandleGet(int clientSocket, json requestJSON,std::string requestRoute,json headers){
    json responseJSON = {};
    Response::RespondJSON(0,0,responseJSON);
    return 0;
}
```
The HandleGet function must take in:
- clientSocket
- requestJSON
- requestRoute
- JSON headers

### Beyond

Now that you have set up everything you needed to set up, you can start creating your project. Remember, all GET and POST requests will be forwarded to their respective functions.