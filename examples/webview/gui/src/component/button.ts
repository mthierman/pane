declare global {
    interface HTMLElementTagNameMap {
        [Button.tag]: Button;
    }
}

export class Button extends HTMLButtonElement {
    static tag = "pane-button";

    static define() {
        if (!customElements.get(Button.tag)) {
            customElements.define(Button.tag, this, { extends: "button" });
        }
    }

    static new() {
        return document.createElement("button", { is: Button.tag }) as Button;
    }

    connectedCallback() {
        this.addEventListener("click", () => {});
    }
}
