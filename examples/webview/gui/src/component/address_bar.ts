import { Component } from "pane";

declare global {
    interface HTMLElementTagNameMap {
        "pane-address-bar": AddressBar;
    }
}

class AddressBar extends Component {
    static {
        if (!customElements.get("pane-address-bar")) {
            customElements.define("pane-address-bar", AddressBar);
        }
    }

    #form = document.createElement("form");
    #input = document.createElement("input");
    #button = document.createElement("button");
    #url = new URL("about:blank");

    get() {
        return this.#form;
    }

    get input() {
        return this.#input;
    }

    get button() {
        return this.#button;
    }

    get url(): URL {
        return this.#url;
    }

    set url(href: string) {
        this.#url.href = href;
        this.#input.value = this.#url.href;
    }

    connectedCallback() {
        this.appendChild(this.#form);
        this.#form.appendChild(this.#input);
        this.#form.appendChild(this.#button);
        this.#button.type = "submit";
        this.#button.innerText = "↵";

        this.addEventListener("init", (event) => {
            this.url = event.detail.url;
        });

        this.#form.addEventListener("submit", (event) => {
            event.preventDefault();

            if (!/\.\w+$/.test(this.#input.value)) {
                // Is search query
            }

            if (!this.#input.value.startsWith("http")) {
                if (!/\.\w+$/.test(this.#input.value)) {
                    this.#input.value = `${this.#input.value}.com`;
                }
                this.url = `https://${this.#input.value}`;
            }
        });

        // this.#input.addEventListener("change", () => {
        //     if (!this.#input.value.startsWith("http")) {
        //         this.url = `https://${this.#input.value}`;
        //         console.log(this.url);
        //     } else {
        //         this.#input.value = this.url.toString();
        //     }
        // });

        // this.#input.addEventListener("keydown", (event) => {
        //     console.log(`keydown ${event.key}`);
        //     if (event.key === "Enter") {
        //         const test: WebViewMessageEventData<payload> = {
        //             type: "init",
        //             payload: {
        //                 url: this.url.href,
        //             },
        //         };
        //         window.chrome.webview.postMessage(test);
        //     }
        //     this.dispatchEvent();
        // });

        // this.#input.addEventListener("change", () => {
        //     if (!this.#input.value.startsWith("http")) {
        //         this.url = `https://${this.#input.value}`;
        //         console.log(this.url);
        //     } else {
        //         this.#input.value = this.url.toString();
        //     }
        // });
    }
}

export {};
