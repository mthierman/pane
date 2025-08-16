import { App } from "../app";

declare global {
    interface HTMLElementTagNameMap {
        [AddressBar.tag]: AddressBar;
    }
}

export class AddressBar extends HTMLInputElement {
    static tag = "pane-address-bar";

    static define() {
        if (!customElements.get(AddressBar.tag)) {
            customElements.define(AddressBar.tag, this, { extends: "input" });
        }
    }

    static new() {
        AddressBar.define();
        let element = document.createElement("input", { is: AddressBar.tag }) as AddressBar;
        App.addElement(element);
        return element;
    }

    url = new URL("about:blank");

    set(href: string) {
        this.url.href = href;
        this.value = this.url.href;
    }

    connectedCallback() {
        this.type = "text";
        this.addEventListener("init", (event) => {
            this.set(event.detail.url);
        });

        this.addEventListener("submit", () => {
            console.log(this.value);
            this.set(this.value);

            // if (!this.value.startsWith("http")) {
            //     console.log("!http");
            //     this.url = new URL(`https://${this.value}`);
            // } else {
            //     console.log(new URL(this.value));
            //     this.value = this.url.toString();
            // }

            // if (!this.value.startsWith("http")) {
            //     this.url = new URL(`https://${this.value}`);
            // } else {
            //     this.value = this.url.toString();
            // }
            // console.log(this.url);
        });
    }
}
