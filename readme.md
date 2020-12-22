# [=============> What's this? <=============]

This is a Glasscord installer/reinstaller for Visual Studio Code and Discord(not supported at the moment).
The purpose of this program is to copy/create necessary files whenever you execute it in the case that a
software update breaks your glasscord integration. It is also designed for new users that just want to get
glasscord installed without the hassle of manually doing each thing.

This first version I consider it a complete program; however, there are some caveats and features still to
be implemented:

- This first version won't check for the required CSS file in the .vscode folder to make it work (I'm working
  on this feature atm).
- You also need an [extension that lets you inject CSS files into VS Code](https://marketplace.visualstudio.com/items?itemName=be5invis.vscode-custom-css).

For more information about Glasscord [go here](https://github.com/AryToNeX/Glasscord). Thank you AryToNeX for making this amazing piece of software.
There's also an [official installer](https://github.com/AryToNeX/Glasscordify) available but only works on Linux at the moment.

# [============> Prerequisites <============]

- A working 32 or 64 bit Windows computer (no Linux support sadly :( but I might add it in the future).
- Visual Studio Code or Discord installed(I will add discord support in a future update).
- A CSS loader/injector. [Refer to the "What's this?" section to learn more]

# [=============> Installation <=============]

1. Download the [latest zip file](https://github.com/Rikimbili/glasscord_installer/releases/latest/download/glasscord_installer.zip) and extract it anywhere you like.
2. Fully close Visual Studio Code.
3. Open glasscord_installer.exe; if any error is displayed, reopen the file with administrator privilieges.
4. Profit.

Optional: Make a shortcut to the file on your desktop so you dont have to be searching for it whenever
you need it. Make sure you don't COPY the file because glasscord.asar needs to be in the same location
as the executable.

# [===========> Feature Checklist <===========]

- [ ] Integrate a minimal CLI
- [ ] Discord support and ability to choose desired program
- [ ] Ability to create .css file in the vs code user dir if not present
- [ ] Linux support... maybe?
- [ ] Implement a GUI?