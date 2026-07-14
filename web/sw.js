const CACHE_NAME = "chaos-groove-pwa-v1.12";

const FILES_TO_CACHE = [
    "./",
    "./icon.ico",
    "./icon.png",
    "./index.html",
    "./index.js",
    "./index.wasm",
    "./index.data",
    "./manifest.json",
    // add other static files you want cached
];

self.addEventListener("install", (e) => {
  self.skipWaiting();
  e.waitUntil(
    caches.open(CACHE_NAME).then(cache => cache.addAll(FILES_TO_CACHE))
  );
});

// Activate: clear old caches
self.addEventListener('activate', event => {
    event.waitUntil(
        caches.keys().then(keys =>
            Promise.all(keys.filter(key => key !== CACHE_NAME).map(key => caches.delete(key)))
        )
    );
    return self.clients.claim();
});

// Fetch: network-first for navigation (HTML), cache-first for assets
self.addEventListener('fetch', event => {
    if (event.request.mode === 'navigate') {
        // Always try network first for HTML, then use the cached page.
        event.respondWith(
            fetch(event.request).catch(() =>
                caches.match(event.request).then(response =>
                    response || caches.match(new URL('index.html', self.registration.scope).href)
                )
            )
        );
        return;
    }

    if (event.request.method !== 'GET') {
        return;
    }

    // For other requests (CSS, JS, images), use cache-first with background update
    event.respondWith(
        caches.open(CACHE_NAME).then(cache =>
            cache.match(event.request).then(response => {
                const fetchPromise = fetch(event.request).then(networkResponse => {
                    cache.put(event.request, networkResponse.clone());
                    return networkResponse;
                });
                return response || fetchPromise;
            })
        )
    );
});