import type { webview_message, webview_message_payload } from "./webview_message";

export class Button extends HTMLButtonElement {
    static define(tag = "pane-button") {
        customElements.define(tag, this, { extends: "button" });
    }

    connectedCallback() {
        this.onclick = () => {
            console.log("click");

            const init: webview_message<webview_message_payload> = {
                type: "init",
                payload: {
                    name: "",
                    age: 18,
                },
            };

            window.chrome.webview.postMessage(init);

            const test: webview_message<webview_message_payload> = {
                type: "test",
                payload: {
                    one: 23,
                    two: 162,
                },
            };

            window.chrome.webview.postMessage(test);
        };
    }
}
