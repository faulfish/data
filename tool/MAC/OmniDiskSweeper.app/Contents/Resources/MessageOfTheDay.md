---
iconImage: images/omnidisksweeper.png
appName: OmniDiskSweeper
platform: Mac
marketingName: OmniDiskSweeper
subheading: Release Notes: What's new, updated, or fixed
feedbackEmail: softwareversionsupdater@omnigroup.com
svn-header: $Header$
test: false
key_html: <div class="key"><ul><li class="new">New</li><li class="updated">Updated</li><li class="fixed">Fixed</li></ul></div>
archive: MessageOfTheDayArchive.html
---

### {class:key} Version 1.9 --- Monday, Dec 30, 2013
- updated - **More Responsive** --- OmniDiskSweeper is much more responsive when scanning a slow disk.  (The interface used to block while scanning the list of files contained in a folder, which could cause very long pauses when scanning slow disks.)
- updated - **Faster Scans of Remote Disks** --- Switched to scanning directory contents using API that only collects the small set of attributes that OmniDiskSweeper is actually going to use, so it can scan remote disks much more quickly.
- updated - **Smarter Scan Order** --- Rather than scanning a directory to completion before starting on any other directories, OmniDiskSweeper will automatically skip between folders so you can more quickly get a sense of how big each directory is relative to the others.
- updated - **System Requirements** --- The minimum system requirement is now 10.8 (Mountain Lion).

### {class:key} Version 1.8 --- Monday, July 23, 2012
- new - **64-bit** --- OmniDiskSweeper is now a 64-bit app, which means it can handle much larger sets of files.
- new - **Gatekeeper** --- The app is now signed with Developer ID so it works well with Gatekeeper on Mountain Lion.
- updated - **System Requirements** --- The minimum system requirement is now 10.6.8 (Snow Leopard).
- updated - **Decimal Sizes** --- File sizes are now reported using standard metric prefixes (1 kB = 10^3 bytes = 1,000 bytes; 1 MB = 10^6 bytes = 1,000,000 bytes) rather than using binary-based prefixes (1 kB = 2^10 bytes = 1,024 bytes; 1 MB = 2^20 bytes = 1,048,576 bytes).
- updated - **Interface** --- OmniDiskSweeper has a new app icon!
- fixed - **Lion Automounts** --- Lion's automount folders are no longer displayed in the network drive list.
