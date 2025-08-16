import { Component } from "./component";

declare global {
    interface HTMLElementTagNameMap {
        "pane-button": Button;
    }
}

export class Button extends Component {
    #button = document.createElement("button") as HTMLButtonElement;

    get() {
        return this.#button;
    }

    connectedCallback() {
        this.appendChild(this.#button);

        this.addEventListener("click", () => {});
    }

    static {
        if (!customElements.get("pane-button")) {
            customElements.define("pane-button", Button);
        }
    }
}
