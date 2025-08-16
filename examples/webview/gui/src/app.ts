export type ComponentEventMap<T extends Record<string, any>> = {
    [K in keyof T]: CustomEvent<T[K]>;
};

export const App = {
    html: document.documentElement as HTMLHtmlElement,
    body: document.body as HTMLBodyElement,

    addElement<T extends Node>(element: T) {
        return this.body.appendChild(element);
    },

    getElementById<T = HTMLElement>(elementId: string) {
        return document.getElementById(elementId) as T | null;
    },
};
