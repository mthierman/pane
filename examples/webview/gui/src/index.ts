import { AddressBar, Button } from "./component";

import "./style/index.css";

export type payload = {
    init: { url: string };
    test: { one: number; two: number };
};

declare global {
    interface HTMLElementEventMap extends WebViewMessageEventMap<payload> {}
}
let address_bar = AddressBar.new();
let button = Button.new();
button.get().innerText = "↵";

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
