import "./index.css";

import type { AddressBar } from "./address_bar";
import type { Button } from "./button";
import { getElementById } from "./utility";

export function getElements() {
    return {
        button: getElementById<Button>("button"),
        address_bar: getElementById<AddressBar>("address-bar"),
    };
}

export type payload = {
    init: { name: string; age: number };
    test: { one: number; two: number };
};

window.chrome.webview.addEventListener<payload>("message", (event) => {
    console.log(event.data);
    const element = getElements();

    switch (event.data.type) {
        case "init":
            {
                if (element.button) {
                    element.button.innerText = event.data.payload.name;
                }

                if (element.address_bar) {
                    element.address_bar.value = event.data.payload.name;
                }
            }
            break;
        case "test":
            {
                console.log(event.data);
            }
            break;
    }
});
