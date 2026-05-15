// FIXME: This file should handle the auth redirection
// Get the code from the URL parameters and redirect to the relevant page
//
import { getToken } from "../../../utils/auth";
import { redirectToLoginPage } from "../../../utils/redirect";

(async () => {
    const params = new URLSearchParams(window.location.search);
    const code = params.get("code");

    if (!code) {
        redirectToLoginPage();
        return;
    }

    const success = await getToken(code);

    if (success) {
        window.location.href = `${import.meta.env.VITE_URL}/`;
    } else {
        redirectToLoginPage();
    }
})();
