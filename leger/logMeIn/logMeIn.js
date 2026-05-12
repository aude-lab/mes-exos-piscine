const express = require("express");
const jwt = require("jsonwebtoken");

require("dotenv").config();

function logMeIn(host, port) {
    const app = express();

    app.use(express.json());

    app.get("/", (req, res) => {
        res.status(200).json({ message: "Hello World!" });
    });

    app.post("/login", (req, res) => {
        const { username, password } = req.body;

        /*if (!username || !password) {
            return res.status(400).json({
                error: "Missing credentials",
                message: "Both username and password are required",
            });
        }*/

        if (username === "xavier.login" && password === "1234") {
            const token = jwt.sign(
                {
                    username: "xavier.login",
                    password: "1234",
                },
                process.env.JWT_SECRET_KEY,
            );

            res.status(200).json({ jwt: token });
        } else {
            return res
                .status(401)
                .json({ error: "Invalid username or password" });
        }
    });

    app.get("/secret", (req, res) => {
        const authHeader = req.headers["authorization"];

        if (!authHeader || !authHeader.startsWith("Bearer ")) {
            return res.status(401).json({ error: "Unauthorized" });
        }

        const token = authHeader.split(" ")[1];

        if (!token) {
            return res.status(401).json({ error: "Unauthorized" });
        }

        try {
            jwt.verify(token, process.env.JWT_SECRET_KEY);

            res.status(200).json({ message: "Access granted" });
        } catch {
            return res.status(401).json({ error: "Unauthorized" });
        }
    });

    const server = app.listen(port, host, () => {
        console.log(`Server running at http://${host}:${port}/`);
    });

    return server;
}

module.exports = { logMeIn };
