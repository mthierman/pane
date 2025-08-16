declare global {
    interface HTMLElementTagNameMap {
        "pane-address-bar": AddressBar;
    }
}

export class AddressBar extends HTMLInputElement {
    static define(tag = "pane-address-bar") {
        customElements.define(tag, this, { extends: "input" });
    }

    url = new URL("about:blank");

    connectedCallback() {
        this.addEventListener("initialize", (event) => {
            this.url.href = event.detail.url;
            this.value = this.url.href;
        });

        this.addEventListener("input", () => {
            if (!this.value.startsWith("http")) {
                this.url = new URL(`https://${this.value}`);
            } else {
                this.value = this.url.toString();
            }
            console.log(this.url);
        });
    }
}
