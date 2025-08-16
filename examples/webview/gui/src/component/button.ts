declare global {
    interface HTMLElementTagNameMap {
        "pane-button": Button;
    }
}

export class Button extends HTMLButtonElement {
    static tag = "pane-address-bar";
    static define() {
        customElements.define(Button.tag, this, { extends: "button" });
    }

    connectedCallback() {
        this.addEventListener("click", () => {});
    }
}
