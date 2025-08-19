import { Page } from "pane";
import "./component/address_bar";
import "./component/button";
import "./styles/index.css";
import "./styles/reset.css";

export type event_payload = {
    init: { url: string };
    test: { hello_world: string };
};

declare global {
    interface HTMLElementEventMap extends WebViewMessageEventMap<event_payload> {}
}

const app = {
    address_bar: Page.createElement("pane-address-bar").with_id("address-bar").attach(),
};

window.chrome.webview.addEventListener<event_payload>("message", (event) => {
    switch (event.data.type) {
        case "init":
            {
                app.address_bar.dispatchWebViewEvent(event);
            }
            break;
        case "test":
            {
                console.log(event.data.payload);
            }
            break;
    }
});
