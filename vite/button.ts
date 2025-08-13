import { type init } from "./comms";

export class Button extends HTMLButtonElement {
    static define(tag = "pane-button") {
        customElements.define(tag, this, { extends: "button" });
    }

    connectedCallback() {
        this.onclick = () => {
            const init: init = {
                type: "init",
                payload: { name: "Jackson Mays", age: 24 },
            };

            window.chrome.webview.postMessage(init);
        };
    }
}

Button.define();
