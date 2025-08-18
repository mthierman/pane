import { Page } from "pane";
import "./components";
import "./index.css";
import "./reset.css";

export type payload = {
    init: { url: string };
    test: { one: number; two: number };
};

declare global {
    interface HTMLElementEventMap extends WebViewMessageEventMap<payload> {}
}

let address_bar = Page.createElement("pane-address-bar");
address_bar.append(Page.body());
// Page.appendToBody(address_bar);
// address_bar.appendToParent(address_bar, address_bar);

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
