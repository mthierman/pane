import type { webview_message } from "./event";

export class Button extends HTMLButtonElement {
    static define(tag = "pane-button") {
        customElements.define(tag, this, { extends: "button" });
    }

    connectedCallback() {
        this.onclick = () => {
            console.log("click");

            const init: webview_message = {
                type: "init",
                payload: {
                    name: "Cindy Thompson",
                    age: 18,
                },
            };

            window.chrome.webview.postMessage(init);

            const test: webview_message = {
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
