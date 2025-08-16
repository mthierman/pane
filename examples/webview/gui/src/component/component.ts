import { AddressBar } from "./address_bar";
import { Button } from "./button";

export const body = document.body as HTMLBodyElement;

export const html = document.documentElement as HTMLHtmlElement;

export function getElementById<T = HTMLElement>(elementId: string) {
    return document.getElementById(elementId) as T | null;
}

export class Component extends HTMLElement {
    static define(name: string, constructor: CustomElementConstructor) {
        if (!customElements.get(name)) {
            customElements.define(name, constructor);
        }
    }

    attach<T extends Node>(element: T) {
        return body.appendChild(element);
    }

    dispatch<T>(event: WebViewMessageEvent<T>) {
        super.dispatchEvent(
            new CustomEvent(event.data.type, {
                detail: event.data.payload,
            }),
        );
    }
}

export { AddressBar, Button };
