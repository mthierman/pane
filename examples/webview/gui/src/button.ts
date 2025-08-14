import type { webview_message_payload } from "./webview_message";

export class Button extends HTMLButtonElement {
    static define(tag = "pane-button") {
        customElements.define(tag, this, { extends: "button" });
    }

    connectedCallback() {
        this.onclick = () => {
            console.log("click");
            // const init: webview_message_payload["init"] = {
            //     type: "init",
            //     payload: { name: "Jackson Mays", age: 24 },
            // };
            // window.chrome.webview.postMessage(init);
        };
    }
}
