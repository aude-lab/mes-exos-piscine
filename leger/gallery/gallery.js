const fs = require("fs");
const path = require("path");

function extract(directoryPath) {
    if (!fs.existsSync(directoryPath)) {
        throw new Error("The directory does not exist");
    }

    const files = fs.readdirSync(directoryPath);
    const emails = [];

    files.forEach((file) => {
        const filePath = path.join(directoryPath, file);
        const content = fs.readFileSync(filePath, "utf8");
        const emailRegex = /[a-z0-9_.+-]+@[a-z0-9.-]+\.[a-z.]{2,}/g;
        const matches = content.match(emailRegex);

        if (matches) {
            emails.push(...matches);
        }
    });

    return emails;
}

module.exports = { extract };
