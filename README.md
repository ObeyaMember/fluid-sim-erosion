- code --enable-features=UseOzonePlatform --ozone-platform=wayland
    -   uhh some command to run VSCode with to avoid getting this error:
        - libdecor-gtk-WARNING: Failed to initialize GTK
          Failed to load plugin 'libdecor-gtk.so': failed to init
- Apparently vscode defaults to using X11 protocol instead of wayland

- OOOR run the executable from the terminal 







 - Formulation sujet TIPE:
  - Modélisation de l'érosion hydraulique sur un relief

- Etapes:
  - Modélisation basique d'un fluide
      - Viscosité
      - Forces réciproques
      - Emploi d'une meilleure formule forces de répulsions formule
  - Modélisation du relief par une heightmap
  - Modélisation de l'intéraction fluide - relief
  - 1ère amélioration de la modélisation de fluide + érosion: Hash grid
  - 2ème amélioration de la modélisation de fluide + érosion: Parallélisation des calculs avec radix sort
  Idées d'améliorations possibles:
    - Remplacer la heightmap par une grille de voxel -> falaises