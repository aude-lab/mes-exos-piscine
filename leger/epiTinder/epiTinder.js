const fs = require("fs");
const express = require("express");

function readUsersFromJSONFile(JSON_filename) {
    /*
     ** Return the list of users stored in the JSON file
     ** JSON_filename: path to the JSON file
     */
    const content = fs.readFileSync(JSON_filename, (err) => {
        if (err) {
            console.error(err);
            return;
        }
    });

    return JSON.parse(content).users;
}

function writeUsersToJSONFile(JSON_filename, users) {
    /*
     ** Overwrite the given JSON_filename with the given
     ** list of users.
     ** JSON_filename: path to the JSON file
     ** users : list of users objects
     */
    const usersJSON = JSON.stringify({ users: users });

    fs.writeFileSync(JSON_filename, usersJSON, (err) => {
        if (err) {
            console.error(err);
            return;
        }
    });
}

function epiTinderWebServer(host, port, filename) {
    const app = express();

    app.use(express.json());

    app.get("/", (req, res) => {
        res.status(200).json({ message: "Hello World!" });
    });

    app.get("/users", (req, res) => {
        const users = readUsersFromJSONFile(filename);

        res.status(200).json(users);
    });

    app.post("/users", (req, res) => {
        const users = readUsersFromJSONFile(filename);
        const newId =
            users.length > 0 ? Math.max(...users.map((u) => u.id)) + 1 : 0;
        const newUser = { id: newId, ...req.body };

        users.push(newUser);
        writeUsersToJSONFile(filename, users);
        res.status(201).json(newUser);
    });

    app.get("/users/:id", (req, res) => {
        const users = readUsersFromJSONFile(filename);
        const user = users.find((user) => user.id === parseInt(req.params.id));

        if (!user) {
            res.status(404).json({
                message: `No user with id: ${req.params.id} found`,
            });
        } else {
            res.status(200).json(user);
        }
    });

    app.put("/users/:id", (req, res) => {
        const users = readUsersFromJSONFile(filename);
        const userIndex = users.findIndex(
            (user) => user.id === parseInt(req.params.id),
        );

        if (userIndex === -1) {
            return res
                .status(404)
                .json({ message: `No user with id: ${req.params.id} found` });
        } else {
            const updatedUser = { id: parseInt(req.params.id), ...req.body };

            users[userIndex] = updatedUser;
            writeUsersToJSONFile(filename, users);
            res.status(201).json(updatedUser);
        }
    });

    app.delete("/users/:id", (req, res) => {
        const users = readUsersFromJSONFile(filename);
        const userIndex = users.findIndex(
            (user) => user.id === parseInt(req.params.id),
        );

        if (userIndex === -1) {
            return res
                .status(404)
                .json({ message: `No user with id: ${req.params.id} found` });
        } else {
            const deletedUser = users.splice(userIndex, 1)[0];

            writeUsersToJSONFile(filename, users);
            res.status(200).json(deletedUser);
        }
    });

    app.use((req, res) => {
        res.status(404).json({ message: "Not found" });
    });

    const server = app.listen(port, host, () => {
        console.log(`Server running at http://${host}:${port}/`);
    });

    return server;
}

module.exports = { epiTinderWebServer };
