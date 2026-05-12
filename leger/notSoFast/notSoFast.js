const fetch = require("node-fetch");

async function notSoFast(host, port) {
    const baseUrl = `http://${host}:${port}`;

    async function fetchData(url) {
        const response = await fetch(url, { method: "GET" });
        const body = await response.text();

        return {
            statusCode: response.status,
            headers: Object.fromEntries(response.headers),
            body,
        };
    }

    function delay(duration) {
        return new Promise((resolve) => setTimeout(resolve, duration));
    }

    const countResponse = await fetchData(`${baseUrl}/articles`);

    if (countResponse.statusCode !== 200) {
        throw new Error(
            `Failed to get articles count: ${countResponse.statusCode}`,
        );
    }

    const countData = JSON.parse(countResponse.body);
    const totalArticles = countData.message;

    const articlesList = [];
    let remainingRequests = null;
    let resetTime = null;

    for (let id = 0; id < totalArticles; id++) {
        const currentTime = Date.now();

        if (remainingRequests !== null && resetTime !== null) {
            if (remainingRequests <= 0 && currentTime < resetTime) {
                const waitDuration = resetTime - currentTime + 5;

                await delay(waitDuration);
            }
        }

        const articleResponse = await fetchData(`${baseUrl}/articles/${id}`);

        if (articleResponse.statusCode === 429) {
            await delay(100);
            id--;
            continue;
        }

        if (articleResponse.statusCode !== 200) {
            throw new Error(
                `Failed to get article ${id}: ${articleResponse.statusCode}`,
            );
        }

        const articleData = JSON.parse(articleResponse.body);

        articlesList.push(articleData);

        if (articleResponse.headers["x-ratelimit-remaining"]) {
            remainingRequests = parseInt(
                articleResponse.headers["x-ratelimit-remaining"],
            );
        }

        if (articleResponse.headers["x-ratelimit-reset"]) {
            resetTime =
                parseFloat(articleResponse.headers["x-ratelimit-reset"]) * 1000;
        }

        if (
            id < totalArticles - 1 &&
            (remainingRequests === null || remainingRequests > 0)
        ) {
            await delay(1);
        }
    }

    return articlesList;
}

module.exports = { notSoFast };
