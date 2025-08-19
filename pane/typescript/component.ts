import { Page } from "./page";

declare global {
    interface CustomElementRegistry {
        get<K extends keyof HTMLElementTagNameMap>(
            name: K,
        ): (new () => HTMLElementTagNameMap[K]) | undefined;
        define<K extends keyof HTMLElementTagNameMap>(
            name: K,
            constructor: new () => HTMLElementTagNameMap[K],
            options?: ElementDefinitionOptions,
        ): void;
    }
}

export class Component extends HTMLElement {
    attach<T extends Node>(parent?: T) {
        return (parent ?? Page.body).appendChild(this);
    }

    dispatch<T = unknown>(event: WebViewMessageEvent<T>) {
        super.dispatchEvent(
            new CustomEvent(event.data.type, {
                detail: event.data.payload,
            }),
        );
    }
}
