import { calculateLayout } from "./utils";
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
    const { colors, metadata } = config;

    const roomNameElem = document.getElementById("room-name");
    const roomDescriptElem = document.getElementById("room-description");
    if (roomNameElem) roomNameElem.textContent = metadata?.name ?? roomSlug;
    if (roomDescriptElem) {
        if (metadata?.description) {
            roomDescriptElem.textContent = metadata.description;
            roomDescriptElem.style.display = "";
        } else {
            roomDescriptElem.style.display = "none";
        }
    }

    const canvasResponse = await authedAPIRequest(`/rooms/${roomSlug}/canvas`, { method: "GET" });
    if (!canvasResponse || !canvasResponse.ok) return;

    const canvasBuffer = await canvasResponse.arrayBuffer();
    const encoded = new Uint8Array(canvasBuffer);
    const totalPixels = metadata.canvasDimensions * metadata.canvasDimensions;
    const decoded = decodecanva(encoded, totalPixels);

    initCanvas(decoded, colors, metadata.canvasDimensions);

    for (const update of pendingUpdates) {
        renderCanvasUpdate(update, colors);
    }

    socket.off("pixel-update");
    socket.on("pixel-update", (data) => {
        renderCanvasUpdate(data, colors);
    });
})();



function decodecanva(data, totalPixels) {
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
