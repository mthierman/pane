import type { payload } from "./payload";

export class Button extends HTMLButtonElement {
    static define(tag = "pane-button") {
        customElements.define(tag, this, { extends: "button" });
    }

    connectedCallback() {
        this.addEventListener("click", () => {
            const init: WebViewMessageEventData<payload> = {
                type: "init",
                payload: {
                    name: "Cindy Thompson",
                    age: 18,
                },
            };

            window.chrome.webview.postMessage(init);

            const test: WebViewMessageEventData<payload> = {
                type: "test",
                payload: {
                    one: 23,
                    two: 162,
                },
            };

            window.chrome.webview.postMessage(test);
        });
    }
}

Button.define();
