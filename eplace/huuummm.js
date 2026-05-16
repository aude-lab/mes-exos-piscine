Ce timeout vient du test Jest qui importe streams.js directement. Le await new Promise qui attend connect bloque indéfiniment dans un environnement de test où il n'y a pas de vrai serveur.
La solution c'est de ne pas attendre le connect dans initSocket mais plutôt de retourner une Promise qui se résout quand le socket est prêt, sans bloquer si ça échoue :
jsexport async function initSocket() {
    if (socket) return;

    const isAuthentif = await authenticate();
    if (!isAuthentif) return;

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
Sans le await new Promise du tout. Le socket se connecte en arrière-plan et subscribe attend naturellement que la connexion soit établie avant d'envoyer des messages.
