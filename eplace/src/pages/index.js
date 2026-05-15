// FIXME: This is the entry point of the application, write your code here

import { calculateLayout } from "./utils";
import { authenticate } from "../utils/auth";
// Initialize the layout
calculateLayout();
(async () => {
    await authenticate();
})();
