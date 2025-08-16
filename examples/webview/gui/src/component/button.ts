declare global {
    interface HTMLElementTagNameMap {
        "pane-button": Button;
    }
}

export class Button extends HTMLButtonElement {
    static tag = "pane-button";

    static define() {
        customElements.define(Button.tag, this, { extends: "button" });
    }

    static new() {
        return document.createElement("pane-button", { is: Button.tag });
    }

    connectedCallback() {
        this.addEventListener("click", () => {});
    }
}
