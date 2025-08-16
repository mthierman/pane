export const Utility = {
    html: document.documentElement as HTMLHtmlElement,
    body: document.body as HTMLBodyElement,

    getElementById<T = HTMLElement>(elementId: string) {
        return document.getElementById(elementId) as T | null;
    },
};
