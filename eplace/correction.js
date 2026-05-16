export function subscribe(roomSlug) {
    return new Promise((resolve) => {
        const subscriptionId = uuidv4();

        const handler = (message) => {
            if (message?.id === subscriptionId && message?.result?.type === "started") {
                socket.off("message", handler);
                resolve();
            }
        };

        socket.on("message", handler);

        socket.emit("message", {
            id: subscriptionId,
            method: "subscription",
            params: {
                path: "rooms.canvas.getStream",
                input: {
                    json: {
                        roomSlug: roomSlug,
                    },
                },
            },
        });
    });
}
Et le unsubscribe aussi à corriger :
jsexport function unsubscribe(roomSlug) {
    if (!socket) return;
    socket.emit("message", {
        id: uuidv4(),
        method: "subscription.stop",
    });
}
