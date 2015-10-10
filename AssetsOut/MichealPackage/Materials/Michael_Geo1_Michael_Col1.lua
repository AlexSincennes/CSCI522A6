function fillMaterialTable(args) -- the script format requires existence of this function
args['version']=2
args['diffuse']={1.000000, 1.000000, 1.000000, 1.000000}
args['shininess']=5.000000
args['reflectivity']={0.000000, 0.000000, 0.000000}
args['specular']={0.200000, 0.200000, 0.200000}
args['emissive']={0.400000, 0.400000, 0.400000}
args['textures']={
  {'NORMAL', 'michael_diff.png'},
  {'SPECULAR', 'michael_diff.png'},
  {'GLOW', 'michael_diff.png'},
} -- end textures
--notes overrides
args['textures']={
{'COLOR', 'michael_diff.dds'},
{'SPECULAR', 'michael_diff.dds'},
{'NORMAL', 'michael_diff.dds'},
{'GLOW', 'michael_diff.dds'},
}  -- end textures
--notes overrides

--notes end

end
