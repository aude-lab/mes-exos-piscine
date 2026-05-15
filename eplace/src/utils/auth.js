// FIXME: This file should handle the authentication
// Functions may include:
// - getToken (exchanges the code for a token)
// - refreshToken (refreshes the token using the refresh_token)
// - authenticate (checks if the user is authenticated)
// - authedAPIRequest (makes an authenticated request to the API)
//
import { redirectToLoginPage } from "./redirect";

const TOKEN_KEY = "token";
const REFRESH_TOKEN_KEY = "refresh_token";

const TOKEN_ENDPOINT = `${import.meta.env.VITE_AUTH_URL}/token`;
const API_URL = `${import.meta.env.VITE_URL}/api`;





async function sendTokenRequest(formData) {
    try {
        const response = await fetch(TOKEN_ENDPOINT, {
            method: "POST",
            body: formData,
        });

        if (!response.ok) {
            localStorage.clear();
            redirectToLoginPage();
            return false;
        }

        const data = await response.json();
        localStorage.setItem(TOKEN_KEY, data.id_token);
        localStorage.setItem(REFRESH_TOKEN_KEY, data.refresh_token);
        return true;
    } catch {
        localStorage.clear();
        redirectToLoginPage();
        return false;
    }
}





export async function getToken(code) {
    const formData = new FormData();
    formData.append("grant_type", "authorization_code");
    formData.append("code", code);
    formData.append(
        "redirect_uri",
        `${import.meta.env.VITE_URL}/complete/epita/`
    );
    formData.append("client_id", import.meta.env.VITE_CLIENT_ID);

    return sendTokenRequest(formData);
}





export async function refreshToken(refreshTokenValue) {
    const token = refreshTokenValue ?? localStorage.getItem(REFRESH_TOKEN_KEY);

    if (!token) {
        localStorage.clear();
        redirectToLoginPage();
        return false;
    }

    const formData = new FormData();
    formData.append("grant_type", "refresh_token");
    formData.append("refresh_token", token);
    formData.append("client_id", import.meta.env.VITE_CLIENT_ID);

    return sendTokenRequest(formData);
}





export async function authenticate() {
    const token = localStorage.getItem(TOKEN_KEY);

    if (token) {
        return true;
    }

    const storedRefreshToken = localStorage.getItem(REFRESH_TOKEN_KEY);
    if (storedRefreshToken) {
        return refreshToken(storedRefreshToken);
    }

    redirectToLoginPage();
    return false;
}




export async function authedAPIRequest(endpoint, options) {
    const isAuth = await authenticate();
    if (!isAuth) {
        return null;
    }

    const token = localStorage.getItem(TOKEN_KEY);

    const response = await fetch(`${API_URL}${endpoint}`, {
        ...options,
        headers: {
            ...(options.headers ?? {}),
            Authorization: `Bearer ${token}`,
        },
    });

    if (response.status === 401) {
        const text = await response.clone().text();

        if (text.includes("Token expired")) {
            const refreshed = await refreshToken();
            if (!refreshed) return null;

            const newToken = localStorage.getItem(TOKEN_KEY);
            return fetch(`${API_URL}${endpoint}`, {
                ...options,
                headers: {
                    ...(options.headers ?? {}),
                    Authorization: `Bearer ${newToken}`,
                },
            });
        }

        localStorage.clear();
        redirectToLoginPage();
        return null;
    }

    return response;
}
