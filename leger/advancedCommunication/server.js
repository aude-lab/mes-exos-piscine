const WebSocket = require("ws");

function startServer() {
    let nextId = 1;
    const clients = new Map();

    const wss = new WebSocket.Server({ port: 8080 });

    console.log("Websocket server is running on port 8080.");

    wss.on("connection", (ws, request) => {
        const userName = new URLSearchParams(request.url.split("?")[1]).get(
            "username",
        );

        const userID = nextId++;

        console.log(`[${userID}] ${userName}: trying to establish connection`);

        if (
            !userName ||
            [...clients.values()].some((client) => client.userName === userName)
        ) {
            const errorMsg = `Username: "${userName}" is already taken`;

            console.log(`[${userID}] Disconnected: [1008] ${errorMsg}`);
            ws.close(1008, errorMsg);
            return;
        }

        clients.set(ws, { id: userID, userName });

        /*console.log(`[${userID}] ${userName}: trying to establish connection`);*/
        ws.send(`Welcome ${userName}`);
        const totalPlayers = clients.size;

        if (totalPlayers === 1) {
            ws.send(`${userName}, you are the only player connected`);
        } else {
            ws.send(`${totalPlayers} players are connected`);
        }

        broadcast(`${userName} connected`, ws);

        ws.on("message", (message) => {
            if (!message.includes(": trying to establish connection")) {
                console.log(`[${userID}] ${message}`);
            }
        });

        ws.on("close", (code, reason) => {
            console.log(`[${userID}] Disconnected: [${code}] ${reason}`);
            clients.delete(ws);
            broadcast(`${userName} disconnected`);
        });

        if (totalPlayers === 4) {
            wss.clients.forEach((client) => {
                const { userName } = clients.get(client);

                client.send(
                    `Match will start soon, disconnecting ${userName} from the lobby`,
                );
                client.close(1000, "Match is starting");
            });
        } else {
            broadcast(
                `Waiting for ${4 - totalPlayers} other players to start the game`,
            );
        }
    });
    function broadcast(message, sender = null) {
        wss.clients.forEach((client) => {
            if (client !== sender && client.readyState === WebSocket.OPEN) {
                client.send(message);
            }
        });
    }

    return wss;
}

module.exports = { startServer };
