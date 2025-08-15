import { Button } from "./button";
import type { event } from "./event";
import "./index.css";

Button.define();
const button = document.getElementById("button") as Button;

window.chrome.webview.addEventListener<event>("message", (event) => {
    switch (event.data.type) {
        case "init":
            {
                console.log(event.data);
            }
            break;
        case "test":
            {
                console.log(event.data);
            }
            break;
    }
});
