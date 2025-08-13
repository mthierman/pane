import { type init_data } from "./comms";

export class Button extends HTMLButtonElement {
    // button = document.createElement("button");

    static define(tag = "pane-button") {
        customElements.define(tag, this, { extends: "button" });
    }

    connectedCallback() {
        // this.button.textContent = "Click me";
        // this.onclick = () => console.log("Button clicked!");
        this.onclick = () => {
            const init_data: init_data = {
                type: "init_data",
                payload: { name: "Jackson Mays", age: 24 },
            };
            window.chrome.webview.postMessage(init_data);
        };
        // this.appendChild(this.button);
    }
}

Button.define();
