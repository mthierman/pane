// import type { payload } from "./index";

export class Button extends HTMLButtonElement {
    static define(tag = "pane-button") {
        customElements.define(tag, this, { extends: "button" });
    }

    connectedCallback() {
        this.addEventListener("click", () => {});
    }
}

Button.define();
