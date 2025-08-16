import { Component } from "./component";

declare global {
    interface HTMLElementTagNameMap {
        "pane-address-bar": AddressBar;
    }
}

export class AddressBar extends Component {
    static {
        if (!customElements.get("pane-address-bar")) {
            customElements.define("pane-address-bar", AddressBar);
        }
    }

    #form = document.createElement("form");
    #input = document.createElement("input");
    #url = new URL("about:blank");

    get() {
        return this.#form;
    }

    get input() {
        return this.#input;
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
            } else {
                this.#input.value = this.url.toString();
            }

            // this.url = this.#input.value;
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
