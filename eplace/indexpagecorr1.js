import { calculateLayout } from "./utils";
import { authenticate } from "../utils/auth";
import { initSocket, socket, subscribe } from "../utils/streams";
import { authedAPIRequest } from "../utils/auth";
import { initCanvas, renderCanvasUpdate } from "../rooms/canvas/utils";

calculateLayout();

(async () => {
    const isAuth = await authenticate();
    if (!isAuth) return;

    const pathParts = window.location.pathname.split("/").filter(Boolean);
    const roomSlug = pathParts[0] ?? "epi-place";

    await initSocket();

    const pendingUpdates = [];
    socket.on("pixel-update", (data) => {
        pendingUpdates.push(data);
    });

    await subscribe(roomSlug);

    const configResponse = await authedAPIRequest(`/rooms/${roomSlug}/config`, {
        method: "GET",
    });

    if (!configResponse || !configResponse.ok) return;

    const config = await configResponse.json();

    const roomNameEl = document.getElementById("room-name");
    const roomDescEl = document.getElementById("room-description");

    if (roomNameEl) roomNameEl.textContent = config.metadata?.name ?? roomSlug;
    if (roomDescEl) {
        if (config.metadata?.description) {
            roomDescEl.textContent = config.metadata.description;
            roomDescEl.style.display = "";
        } else {
            roomDescEl.style.display = "none";
        }
    }

    const canvasResponse = await authedAPIRequest(`/rooms/${roomSlug}/canvas`, {
        method: "GET",
    });

    if (!canvasResponse || !canvasResponse.ok) return;

    const canvasBuffer = await canvasResponse.arrayBuffer();
    const encoded = new Uint8Array(canvasBuffer);
    const totalPixels = config.metadata.canvasDimensions * config.metadata.canvasDimensions;
    const decoded = decodeCanvas(encoded, totalPixels);

    // initCanvas attend (roomConfig, pixels)
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

function decodeCanvas(data, totalPixels) {
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
