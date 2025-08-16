declare global {
    interface HTMLElementTagNameMap {
        [Button.tag]: Button;
    }
}

export class Button extends HTMLButtonElement {
    static tag = "pane-button";
    static define() {
        customElements.define(Button.tag, this, { extends: "button" });
    }

    connectedCallback() {
        this.addEventListener("click", () => {});
    }
}
