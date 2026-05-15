// FIXME: This file should handle the redirection to the AUTH URL
// Functions may include:
// - createLink (construct and return the URL to redirect the user to the login page)
// - redirectToLoginPage (redirect the user to the Forge ID login page)
export function createLink() {
    const authUrl = new URL(`${import.meta.env.VITE_AUTH_URL}/authorize`);

    authUrl.searchParams.set("client_id", import.meta.env.VITE_CLIENT_ID);
    authUrl.searchParams.set("response_type", "code");
    authUrl.searchParams.set(
        "redirect_uri",
        `${import.meta.env.VITE_URL}/complete/epita/`
    );
    authUrl.searchParams.set("scope", "epita profile picture");

    return authUrl;
}

export function redirectToLoginPage() {
    window.location.href = createLink().toString();
}
