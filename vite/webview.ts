// https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/javascript/

export interface HostObjectAsyncProxy extends HostObjectAsyncProxyBase {
    sync(): Promise<HostObjectSyncProxy>;
}

export interface HostObjectAsyncProxyBase extends CallableFunction {
    applyHostFunction(argArray?: unknown): Promise<unknown>;
    getHostProperty(propertyName: string): Promise<unknown>;
    getLocalProperty(propertyName: string): unknown;
    setHostProperty(propertyName: string, propertyValue: unknown): Promise<unknown>;
    setLocalProperty(propertyName: string, propertyValue: unknown): unknown;
}

export interface HostObjectsAsyncRoot extends HostObjectAsyncProxyBase {
    options: HostObjectsOptions;
    sync: HostObjectsSyncRoot;
    cancelPromise(promise: Promise<HostObjectAsyncProxy>): void;
    cleanupSome(): void;
}

export interface HostObjectsOptions {
    defaultSyncProxy: boolean;
    forceAsyncMethodMatches: RegExp[];
    forceLocalProperties: string[];
    ignoreMemberNotFoundError: boolean;
    log: (...data: unknown[]) => void;
    shouldPassTypedArraysAsArrays: boolean;
    shouldSerializeDates: boolean;
}

export interface HostObjectsSyncRoot extends HostObjectSyncProxy {}

export interface HostObjectSyncProxy {
    applyHostFunction(argArray?: unknown): unknown;
    async(): HostObjectAsyncProxy;
    getHostProperty(propertyName: string): unknown;
    getLocalProperty(propertyName: string): unknown;
    setHostProperty(propertyName: string, propertyValue: unknown): unknown;
    setLocalProperty(propertyName: string, propertyValue: unknown): unknown;
}

export interface WebViewMessageEvent<T = unknown> extends MessageEvent<T> {
    additionalObjects: ArrayLike<FileSystemFileHandle | FileSystemDirectoryHandle | null>;
    source: WebView & MessageEventSource;
}

export interface SharedBufferReceivedEvent<T = unknown, U = unknown> extends Event {
    data: T;
    additionalData: U;
    source: WebView;
    getBuffer(): ArrayBuffer;
}

export interface WebViewEventMap {
    message: WebViewMessageEvent;
    sharedbufferreceived: SharedBufferReceivedEvent;
}

export interface WebView extends EventTarget {
    hostObjects: HostObjectsAsyncRoot;
    addEventListener<T = unknown>(
        type: "message",
        listener: (event: WebViewMessageEvent<T>) => void,
        options?: boolean | AddEventListenerOptions,
    ): void;
    addEventListener<T = unknown, U = unknown>(
        type: "sharedbufferreceived",
        listener: (event: SharedBufferReceivedEvent<T, U>) => void,
        options?: boolean | AddEventListenerOptions,
    ): void;
    addEventListener(
        type: string,
        listener: EventListenerOrEventListenerObject,
        options?: boolean | AddEventListenerOptions,
    ): void;
    postMessage(message: unknown): void;
    postMessageWithAdditionalObjects(message: unknown, additionalObjects: ArrayLike<unknown>): void;
    releaseBuffer(buffer: ArrayBuffer): void;
    removeEventListener(
        type: string,
        listener: EventListenerOrEventListenerObject,
        options?: boolean | EventListenerOptions,
    ): void;
}

declare global {
    interface Window {
        chrome: {
            webview: WebView;
        };
    }
}
