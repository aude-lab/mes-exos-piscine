await new Promise((resolve) => {
    socket.on("connect", resolve);
    socket.on("connect_error", resolve);
});
