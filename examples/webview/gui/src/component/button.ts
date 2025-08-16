declare global {
    interface HTMLElementTagNameMap {
        "pane-button": Button;
    }
}

export class Button extends HTMLButtonElement {
    static define(tag = "pane-button") {
        customElements.define(tag, this, { extends: "button" });
    }

    connectedCallback() {
        this.addEventListener("click", () => {});
    }
}
