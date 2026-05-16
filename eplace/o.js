src/utils/streams.js
jsimport { io } from "socket.io-client";
import { v4 as uuidv4 } from "uuid";
import { authenticate, refreshToken } from "./auth";
import { redirectToLoginPage } from "./redirect";

export let socket = null;

export async function initSocket() {
    if (socket) return;

    const isAuth = await authenticate();
    if (!isAuth) return;

    const token = localStorage.getItem("token");

    socket = io(import.meta.env.VITE_URL, {
        path: "/socket.io",
        extraHeaders: {
            Authorization: `Bearer ${token}`,
        },
    });

    await new Promise((resolve) => {
        socket.on("connect", resolve);
    });

    socket.on("connect_error", async (err) => {
        const message = err?.message ?? "";

        if (message.includes("Token expired")) {
            const refreshed = await refreshToken();
            if (refreshed) {
                const newToken = localStorage.getItem("token");
                socket.io.opts.extraHeaders = {
                    Authorization: `Bearer ${newToken}`,
                };
                socket.connect();
            } else {
                localStorage.clear();
                redirectToLoginPage();
            }
        } else if (message.includes("Unauthorized") || message.includes("401")) {
            localStorage.clear();
            redirectToLoginPage();
        }
    });
}

export function subscribe(roomSlug) {
    return new Promise(async (resolve, reject) => {
        const subscriptionId = uuidv4();

        const handler = async (message) => {
            if (message?.error?.json?.message === "Token expired") {
                socket.off("message", handler);
                const refreshed = await refreshToken();
                if (refreshed) {
                    const newToken = localStorage.getItem("token");
                    socket.io.opts.extraHeaders = {
                        Authorization: `Bearer ${newToken}`,
                    };
                    socket.disconnect().connect();
                    socket.once("connect", () => subscribe(roomSlug).then(resolve));
                } else {
                    localStorage.clear();
                    redirectToLoginPage();
                    reject();
                }
                return;
            }

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

export function unsubscribe(roomSlug) {
    if (!socket) return;
    socket.emit("message", {
        id: uuidv4(),
        method: "subscription.stop",
    });
}

src/pages/index.js
jsimport { calculateLayout } from "./utils";
import { authenticate, authedAPIRequest } from "../utils/auth";
import { initSocket, socket, subscribe } from "../utils/streams";
import { initCanvas, renderCanvasUpdate } from "../rooms/canvas/utils";

calculateLayout();

(async () => {
    const isAuth = await authenticate();
    if (!isAuth) return;

    const pathParts = window.location.pathname.split("/").filter(Boolean);
    let roomSlug;
    if (pathParts[0]) {
        roomSlug = pathParts[0];
    } else {
        roomSlug = "epi-place";
    }

    await initSocket();

    const pendingUpdates = [];
    socket.on("pixel-update", (data) => {
        pendingUpdates.push(data);
    });

    await subscribe(roomSlug);

    const configurationResponse = await authedAPIRequest(`/rooms/${roomSlug}/config`, { method: "GET" });
    if (!configurationResponse || !configurationResponse.ok) return;
    const config = await configurationResponse.json();

    const roomNameElem = document.getElementById("room-name");
    const roomDescriptElem = document.getElementById("room-description");
    if (roomNameElem) roomNameElem.textContent = config.metadata?.name ?? roomSlug;
    if (roomDescriptElem) {
        if (config.metadata?.description) {
            roomDescriptElem.textContent = config.metadata.description;
            roomDescriptElem.style.display = "";
        } else {
            roomDescriptElem.style.display = "none";
        }
    }

    const canvasResponse = await authedAPIRequest(`/rooms/${roomSlug}/canvas`, { method: "GET" });
    if (!canvasResponse || !canvasResponse.ok) return;

    const canvasBuffer = await canvasResponse.arrayBuffer();
    const encoded = new Uint8Array(canvasBuffer);
    const totalPixels = config.metadata.canvasDimensions * config.metadata.canvasDimensions;
    const decoded = decodeCanva(encoded, totalPixels);

    initCanvas(config, decoded);

    for (const update of pendingUpdates) {
        const { color, posX, posY } = update.result.data.json;
        renderCanvasUpdate(color, posX, posY);
    }

    socket.off("pixel-update");
    socket.on("pixel-update", (data) => {
        const { color, posX, posY } = data.result.data.json;
        renderCanvasUpdate(color, posX, posY);
    });
})();

function decodeCanva(data, totalPixels) {
    const pixels = [];
    let bitBuffer = 0;
    let bitsInBuffer = 0;
    let byteIndex = 0;

    while (pixels.length < totalPixels) {
        while (bitsInBuffer < 5 && byteIndex < data.length) {
            bitBuffer = (bitBuffer << 8) | data[byteIndex++];
            bitsInBuffer += 8;
        }

        if (bitsInBuffer < 5) break;

        bitsInBuffer -= 5;
        const colorIndex = (bitBuffer >> bitsInBuffer) & 0b11111;
        pixels.push(colorIndex);
    }

    return pixels;
}
