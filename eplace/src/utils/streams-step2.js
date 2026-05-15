// FIXME: This file should handle the sockets and the subscriptions
// Exports must include
// - initSocket (initialize the connection to the socket server)
// - socket (variable resulting of initSocket function)

// Functions may include:
// - subscribe (subscribe to a room's stream or chat)
// - unsubscribe (unsubscribe from a room's stream or chat)
// - sendMessage (send a message to a room's chat)
//
import { io } from "socket.io-client";
import { v4 as uuidv4 } from "uuid";
import { authenticate, refreshToken } from "./auth";
import { redirectToLoginPage } from "./redirect";

/**
 * Socket global partagé avec le reste de l'app.
 */
export let socket = null;

/**
 * Initialise la connexion socket après vérification de l'authentification.
 * Ne crée le socket qu'une seule fois.
 * @returns {Promise<void>}
 */
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

/**
 * S'abonne au stream d'une room et attend la confirmation du serveur.
 * @param {string} roomSlug
 * @returns {Promise<void>}
 */
export function subscribe(roomSlug) {
    return new Promise((resolve) => {
        const subscriptionId = uuidv4();

        const handler = (message) => {
            if (message?.type === "started") {
                socket.off("message", handler);
                resolve();
            }
        };

        socket.on("message", handler);

        socket.emit("message", {
            type: "subscribe",
            id: subscriptionId,
            payload: { roomSlug },
        });
    });
}

/**
 * Se désabonne du stream d'une room.
 * @param {string} roomSlug
 */
export function unsubscribe(roomSlug) {
    if (!socket) return;
    socket.emit("message", {
        type: "unsubscribe",
        id: uuidv4(),
        payload: { roomSlug },
    });
}
