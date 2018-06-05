## Overview

Build a simple webserver that implements a subset of the HTTP/1.1 protocol specification

## Client/server protocol

TritonHTTP is a client-server protocol that is layered on top of the reliable stream-oriented transport protocol TCP. Clients issue request messages to the server, and servers reply with response messages. In its most basic form, a single HTTP-level request-reply exchange happens over a single, dedicated TCP connection. The client first connects to the server, sends the HTTP request message, the server replies with an HTTP response, and then the server closes the connection:

Repeatedly setting up and tearing down TCP connections reduces overall network throughput and efficiency, and so HTTP has a mechanism whereby a client can reuse a TCP connection to a given server. The idea is that the client opens a TCP connection to the server, issues an HTTP request, gets an HTTP reply, and then issues another HTTP request on the already open outbound part of the connection. The server replies with the response, and this can continue through multiple request-reply interactions. The client signals the last request by setting a “Connection: close” header. The server indicates that it will not handle additional requests by setting the “Connection: close” header in the response. Note that the client can issue more than one HTTP request without necessarily waiting for full HTTP replies to be returned.

To support clients that do not properly set the “Connection: close” header, the server must implement a timeout mechanism to know when it should close the connection (otherwise it might just wait forever). For this project, you should set a server timeout of 5 seconds, meaning that if the server doesn’t receive a complete HTTP request from the client after 5 seconds, it sends back a “400” client error message and then closes the connection. We will discuss what a 400 client error message is below.

## HTTP messages

HTTP request and response messages are plain-text, consisting of a header section and an optional body section. The header section is separated from the body section with a blank line. The header consists of an initial line (which is different between requests and responses), followed by zero or more key-value pairs. Every line is terminated by a CRLF (carriage-return followed by a line feed). Thus a message looks like:

<initial line>[CRLF]
Key1: Value1[CRLF]
Key2: Value2[CRLF]
Key3: Value3[CRLF]
...
[CRLF]
<optional body...>

## Request Initial Line

The initial line of an HTTP request header has three components:

- The method (in this project that will be GET)
- The URL
- The highest HTTP version that the client supports

The method field indicates what kind of request the client is issuing. The most common is a GET request, which indicates that the client wants to download the content indicated by the URL (described next).

The URL is a pointer to the resource that the client is intersted in. Examples include /images/myimg.jpg and /classes/fall/cs101/index.html.

The version field takes the form HTTP/x.y, where x.y is the highest version that the client supports. For this course we’ll always use 1.1, so this value should be HTTP/1.1.

The fully formed inital request line would thus look something like:

GET /images/myimg.jpg HTTP/1.1

## To build

make


Starter code copyright (c) 2017, 2018 George Porter.  All Rights Reserved.
