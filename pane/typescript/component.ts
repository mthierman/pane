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
    append<T extends Node>(parent: T) {
        return parent.appendChild(this);
    }

    dispatch<T = unknown>(event: WebViewMessageEvent<T>) {
        super.dispatchEvent(
            new CustomEvent(event.data.type, {
                detail: event.data.payload,
            }),
        );
    }
}
