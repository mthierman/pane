import "./index.css";

import { AddressBar } from "./address_bar";
import { Button } from "./button";
import { getElementById } from "./utility";

export function getElements() {
    return {
        address_bar: getElementById<AddressBar>("address-bar"),
        button: getElementById<Button>("submit-button"),
    };
}

export type payload = {
    init: { url: string };
    test: { one: number; two: number };
};

declare global {
    interface HTMLElementEventMap {
        initialize: CustomEvent<payload["init"]>;
    }
}

window.chrome.webview.addEventListener<payload>("message", (event) => {
    // console.log(event.data);
    const element = getElements();

    switch (event.data.type) {
        case "init":
            {
                if (element.address_bar) {
                    // console.log(event.data.payload.url);
                    element.address_bar.dispatchEvent(
                        new CustomEvent("initialize", { detail: event.data.payload }),
                    );
                }
                // console.log(element.address_bar!.url);
            }
            break;
        case "test":
            {
                // console.log(event.data);
            }
            break;
    }
});

// function testMessages() {
//     const init: WebViewMessageEventData<payload> = {
//         type: "init",
//         payload: {
//             name: "Cindy Thompson",
//             age: 18,
//         },
//     };

//     window.chrome.webview.postMessage(init);

//     const test: WebViewMessageEventData<payload> = {
//         type: "test",
//         payload: {
//             one: 23,
//             two: 162,
//         },
//     };

//     window.chrome.webview.postMessage(test);
// }
