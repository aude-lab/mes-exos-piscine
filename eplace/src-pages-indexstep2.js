import { calculateLayout } from "./utils";
import { authenticate, authedAPIRequest } from "../utils/auth";
import { initSocket, socket, subscribe } from "../utils/streams";
import { initCanvas, renderCanvasUpdate } from "../rooms/canvas/utils";

calculateLayout();

Ensuite l'auth + le slug de la room :

(async () => {
    const isAuth = await authenticate();
    if (!isAuth) return;

    const pathParts = window.location.pathname.split("/").filter(Boolean);
    const roomSlug = pathParts[0] ?? "epi-place";


Ensuite le socket + buffer des updates :
    await initSocket();

    const pendingUpdates = [];
    socket.on("pixel-update", (data) => {
        pendingUpdates.push(data);
    });

    await subscribe(roomSlug);


Ensuite le fetch config + mise à jour du DOM :
    const configResponse = await authedAPIRequest(`/rooms/${roomSlug}/config`, { method: "GET" });
    if (!configResponse || !configResponse.ok) return;
    const config = await configResponse.json();
    const { colors, metadata } = config;

    const roomNameEl = document.getElementById("room-name");
    const roomDescEl = document.getElementById("room-description");
    if (roomNameEl) roomNameEl.textContent = metadata?.name ?? roomSlug;
    if (roomDescEl) {
        if (metadata?.description) {
            roomDescEl.textContent = metadata.description;
            roomDescEl.style.display = "";
        } else {
            roomDescEl.style.display = "none";
        }
    }



Ensuite le fetch canvas + décodage + affichage :
    const canvasResponse = await authedAPIRequest(`/rooms/${roomSlug}/canvas`, { method: "GET" });
    if (!canvasResponse || !canvasResponse.ok) return;

    const canvasBuffer = await canvasResponse.arrayBuffer();
    const encoded = new Uint8Array(canvasBuffer);
    const totalPixels = metadata.canvasDimensions * metadata.canvasDimensions;
    const decoded = decodeCanvas(encoded, totalPixels);

    initCanvas(decoded, colors, metadata.canvasDimensions);

    for (const update of pendingUpdates) {
        renderCanvasUpdate(update, colors);
    }

    socket.off("pixel-update");
    socket.on("pixel-update", (data) => {
        renderCanvasUpdate(data, colors);
    });
})();




Et enfin la fonction de décodage :
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
