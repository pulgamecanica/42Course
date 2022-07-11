# SimpleServer
This very SimpleServer will help us understand how to get started and how browser clients make requests.

Working on port 4242, just becase... LOL

***

### How to test this?
Well that's a good question...
Every browser makes the requests differently and they all send different messages.
I encourage you to test this program using Mozilla Firefox, it's the browser that makes the simplest request and it will accept the message that we send.
Although you can test this with really any browser, but don't be surprised if the behaviour is weird.
For example, google chrome does something quite weird, it connects, it sends a message, recieves our message and AGAIN it connects but this time doesn't send anything... preety weird, right?

***

### Getting Started
Start the program

```bash
    make test
```

If for some reason the program fails and it says it can't bind the port 4242 it means that the port 4242 is not available.
In this case you need to kill the process using that port and wait a little...

Use this command to see the process id & kill it
```bash
    lsof -ni | grep 4242
    kill PROCESS_NUMBER
```
After the progam starts you will see something like this:

<img width="361" alt="Screen Shot 2022-05-25 at 12 29 30" src="https://user-images.githubusercontent.com/28810331/170252108-14cb80ef-1845-4cc9-ac64-9b8148bb45a3.png">

---

### Connecting to the Server

#### Via Browser

Go to any browser and visit [http://localhost:4242/](http://localhost:4242/)

Example:
<img width="1164" alt="Screen Shot 2022-05-25 at 12 43 07" src="https://user-images.githubusercontent.com/28810331/170254228-8d7c119a-5e6a-4cf8-bb2f-02b6558de760.png">

---
#### Via telnet

If you prefer, you can use the *telnet* command to emulate a client on the terminal
(you can install telnet on Linux with your package manager and in iOS with HomeBrew aka Brew)

```bash
    telnet localhost 4242
```

Example:
(After the telnet connected it sent a message to the server, the server recieved th message and the server sent a message back and then closed the connection and it's ready to accept another connection)
<img width="846" alt="Screen Shot 2022-05-25 at 12 33 46" src="https://user-images.githubusercontent.com/28810331/170252779-cefd1640-c445-4eb5-aac1-5764eb4e0413.png">
