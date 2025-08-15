import type { payload } from "./index";

export class Button extends HTMLButtonElement {
    static define(tag = "pane-button") {
        customElements.define(tag, this, { extends: "button" });
    }

    static getElementById(elementId: string) {
        const element = document.getElementById(elementId);
        return element instanceof Button ? element : null;
    }

    connectedCallback() {
        this.onclick = () => {
            console.log("click");

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
        };
    }
}
