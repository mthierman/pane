import { Component } from "pane";

declare global {
    interface HTMLElementTagNameMap {
        "pane-button": Button;
    }
}

class Button extends Component {
    static {
        if (!customElements.get("pane-button")) {
            customElements.define("pane-button", Button);
        }
    }

    #button = document.createElement("button") as HTMLButtonElement;

    get() {
        return this.#button;
    }

    connectedCallback() {
        this.appendChild(this.#button);

        this.addEventListener("click", () => {});
    }
}

export {};
