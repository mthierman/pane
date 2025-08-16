import { appendToBody, createElement } from "./component/index";

import "./style/index.css";

export type payload = {
    init: { url: string };
    test: { one: number; two: number };
};

declare global {
    interface HTMLElementEventMap extends WebViewMessageEventMap<payload> {}
}

let address_bar = createElement("pane-address-bar");
let button = createElement("pane-button");
button.get().innerText = "↵";

appendToBody(address_bar);
appendToBody(button);

window.chrome.webview.addEventListener<payload>("message", (event) => {
    switch (event.data.type) {
        case "init":
            {
                address_bar.dispatch(event);
            }
            break;
        case "test":
            {
                // console.log(event.data);
            }
            break;
    }
});
